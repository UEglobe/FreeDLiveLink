// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ILiveLinkSource.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "IMessageContext.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Misc/ScopeLock.h"

#include "FreeD.h"

class FRunnableThread;
class FSocket;
class ILiveLinkClient;
class ISocketSubsystem;

class FREEDLIVELINK_API FFreeDLiveLinkSource : public ILiveLinkSource, public FRunnable
{
public:

	FFreeDLiveLinkSource(FIPv4Endpoint Endpoint);

	virtual ~FFreeDLiveLinkSource();

	// Begin ILiveLinkSource Interface
	
	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;

	virtual bool IsSourceStillValid() const override;

	virtual bool RequestSourceShutdown() override;

	virtual FText GetSourceType() const override { return SourceType; };
	virtual FText GetSourceMachineName() const override { return SourceMachineName; }
	virtual FText GetSourceStatus() const override { return SourceStatus; }
    virtual void Update() override;
	// End ILiveLinkSource Interface

	// Begin FRunnable Interface

	virtual bool Init() override { return true; }
	virtual uint32 Run() override;
	void Start();
	virtual void Stop() override;
	virtual void Exit() override { }

	// End FRunnable Interface
private:

	ILiveLinkClient* Client;

	// Our identifier in LiveLink
	FGuid SourceGuid;

	FMessageAddress ConnectionAddress;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	FIPv4Endpoint DeviceEndpoint;

	// Socket to receive data on
	FSocket* Socket;

	// Subsystem associated to Socket
	ISocketSubsystem* SocketSubsystem;

	// Threadsafe Bool for terminating the main thread loop
	FThreadSafeBool Stopping;

	// Thread to run socket operations on
	FRunnableThread* Thread;

	// Name of the sockets thread
	FString ThreadName;

	// Time to wait between attempted receives
	FTimespan WaitTime;

    // Buffer to receive socket data into
    TArray<uint8> RecvBuffer;

    FCriticalSection currentLock;
    FreeD_D1_t current_data[256];
    int current_cnt[256];
    void UpdateData(FreeD_D1_t& data);
};
